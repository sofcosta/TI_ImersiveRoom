class Particle {

  PVector pos, vel = new PVector(), acc = new PVector();
  PVector prev = new PVector();

  public Particle(float x, float y) {
    pos = new PVector(x, y);
  }

  private void resetPrev() {
    prev.set(pos.x, pos.y);
  }

  private void reset() {
    pos.set(random(width), random(height));
    resetPrev();
    vel.set(0, 0);
    acc.set(0, 0);
  }

  public void update() {
    resetPrev();
    if (pos.x < 0 || pos.x > width) {
      reset();
    }
    if (pos.y < 0 || pos.y > height) {
      reset();
    }
    acc = getCell(pos.x, pos.y);
    vel.add(acc);
    vel.limit(3);
    pos.add(vel);
  }

  public void draw() {
    line(prev.x, prev.y, pos.x, pos.y);
  }
}
